const { Client, GatewayIntentBits, ChannelType } = require('discord.js');
const fs = require('fs');

const client = new Client({
  intents: [GatewayIntentBits.Guilds]
});

const TOKEN = 'MTM5MDUxNjI5OTQ0MDA2NjYzMA.GOcJtC.KPgyCFyl5lwP8nk3I878ggwJlHP3SKtgyUctXU';  // Pon tu token aquí
const GUILD_ID = '1461072728860917973';

client.once('ready', async () => {
  console.log(`Conectado como ${client.user.tag}`);

  const guild = await client.guilds.fetch(GUILD_ID);
  if (!guild) {
    console.log('Servidor no encontrado');
    return;
  }

  // BORRAR CANALES
  console.log('Borrando canales...');
  const channels = await guild.channels.fetch();

  for (const [channelId, channel] of channels) {
    try {
      await channel.delete('Limpieza antes de crear plantilla');
      console.log(`Canal borrado: ${channel.name}`);
    } catch (error) {
      console.error(`Error borrando canal ${channel.name}:`, error);
    }
  }

  // BORRAR ROLES (excepto @everyone)
  console.log('Borrando roles...');
  const roles = await guild.roles.fetch();

  for (const [roleId, role] of roles) {
    if (role.managed) {
      // No borrar roles gestionados por integraciones (bots, etc)
      continue;
    }
    if (role.name === '@everyone') continue;

    try {
      await role.delete('Limpieza antes de crear plantilla');
      console.log(`Rol borrado: ${role.name}`);
    } catch (error) {
      console.error(`Error borrando rol ${role.name}:`, error);
    }
  }

  // CARGAR PLANTILLA DESDE JSON
  const data = JSON.parse(fs.readFileSync('./template.json', 'utf8'));

  // CREAR ROLES
  for (const roleData of data.roles) {
    try {
      await guild.roles.create({
        name: roleData.name,
        color: roleData.color || 'Default',
        hoist: true,
        reason: 'Creación automática desde plantilla'
      });
      console.log(`Rol creado: ${roleData.name}`);
    } catch (error) {
      console.error(`Error creando rol ${roleData.name}:`, error);
    }
  }

  // CREAR CATEGORÍAS
  const categoriesMap = new Map();
  for (const catData of data.categories) {
    try {
      const category = await guild.channels.create({
        name: catData.name,
        type: ChannelType.GuildCategory,
        reason: 'Creación automática desde plantilla'
      });
      categoriesMap.set(catData.name, category);
      console.log(`Categoría creada: ${catData.name}`);
    } catch (error) {
      console.error(`Error creando categoría ${catData.name}:`, error);
    }
  }

  // CREAR CANALES
  for (const chanData of data.channels) {
    try {
      const parentCategory = categoriesMap.get(chanData.category);

      let channelType;
      if (chanData.type === 'text') channelType = ChannelType.GuildText;
      else if (chanData.type === 'voice') channelType = ChannelType.GuildVoice;
      else {
        console.log(`Tipo de canal no soportado: ${chanData.type}, canal ${chanData.name}`);
        continue;
      }

      await guild.channels.create({
        name: chanData.name,
        type: channelType,
        topic: chanData.topic,
        parent: parentCategory ? parentCategory.id : null,
        reason: 'Creación automática desde plantilla'
      });
      console.log(`Canal creado: ${chanData.name}`);
    } catch (error) {
      console.error(`Error creando canal ${chanData.name}:`, error);
    }
  }

  console.log('Plantilla cargada completamente.');
});

client.login(TOKEN);
